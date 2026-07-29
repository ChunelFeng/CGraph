"""
Installed-wheel integration tests for the pycgraph message façade.

Set ``PYCGRAPH_RUN_INTEGRATION=1`` when running these tests against an
installed wheel. Normal source-tree unit discovery skips them.
"""

import os
import unittest


@unittest.skipUnless(
    os.environ.get("PYCGRAPH_RUN_INTEGRATION") == "1",
    "requires an installed pycgraph wheel",
)
class PyCGraphMessageIntegrationTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        import pycgraph

        cls.pycgraph = pycgraph

    def setUp(self):
        self.pycgraph.GMessagePy.clear_messages()

    def tearDown(self):
        self.pycgraph.GMessagePy.clear_messages()

    def test_existing_extension_api_and_message_facade_are_available(self):
        self.assertTrue(hasattr(self.pycgraph, "GNode"))
        self.assertTrue(hasattr(self.pycgraph, "GPipeline"))
        self.assertTrue(hasattr(self.pycgraph, "GMessagePy"))
        self.assertFalse(hasattr(self.pycgraph, "GMessage"))

        self.assertFalse(hasattr(self.pycgraph, "send_message"))
        self.assertFalse(hasattr(self.pycgraph, "recv_message"))
        self.assertFalse(hasattr(self.pycgraph, "pub_message"))
        self.assertFalse(hasattr(self.pycgraph, "sub_message"))

        message_api = self.pycgraph.GMessagePy
        self.assertEqual("pycgraph", message_api.__module__)
        self.assertEqual("pycgraph", message_api.PushStrategy.__module__)
        self.assertEqual("pycgraph", message_api.Error.__module__)
        self.assertTrue(issubclass(message_api.Error, RuntimeError))

        with self.assertRaises(message_api.Error) as context:
            message_api.recv_message("missing", timeout_ms=0)
        self.assertEqual(
            "message topic [missing] not found",
            context.exception.info,
        )

    def test_send_recv_and_pub_sub_stay_in_python_object_space(self):
        message_api = self.pycgraph.GMessagePy
        send_message = object()
        message_api.create_message_topic("send-recv", capacity=1)

        self.assertEqual(
            1, message_api.send_message("send-recv", send_message)
        )
        self.assertIs(
            send_message,
            message_api.recv_message("send-recv", timeout_ms=0),
        )

        first_conn = message_api.bind_message_topic("pub-sub", capacity=1)
        second_conn = message_api.bind_message_topic("pub-sub", capacity=1)
        published_message = object()
        self.assertEqual(
            2, message_api.pub_message("pub-sub", published_message)
        )
        self.assertIs(
            published_message,
            message_api.sub_message(first_conn, timeout_ms=0),
        )
        self.assertIs(
            published_message,
            message_api.sub_message(second_conn, timeout_ms=0),
        )


if __name__ == "__main__":
    unittest.main()
